import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    
    user_id = session["user_id"]
    stocks = db.execute("SELECT symbol, SUM(shares) AS shares, price FROM transactions WHERE user_id = ? GROUP BY symbol", user_id)
    cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]
    total = 0

    # Calculate price and total value of each stock that the user owns
    for stock in stocks:
        stock["price"] = lookup(stock["symbol"])["price"]
        total += stock["shares"] * stock["price"]

    return render_template("index.html", stocks=stocks, cash=cash, total=total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":

        # Look up symbol
        stock = lookup(request.form.get("symbol"))

        # Check if shares value is a positive integer
        try:
            shares = int(request.form.get("shares"))
        except:
            return apology("integer values only", 400)

        if shares < 0:
            return apology("positive values only", 400)

        # Check if symbol was found
        if stock == None:
            return apology("symbol not found", 400)

        # Calculate transaction value and user's cash balance
        transaction_value = shares * stock["price"]
        user_id = session["user_id"]
        cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]

        # Check if user has enough cash
        if cash < transaction_value:
            return apology("insufficient funds", 400)

        # Update cash balance in users table
        new_cash_balance = cash - transaction_value
        db.execute("UPDATE users SET cash = ? WHERE id = ?", new_cash_balance, user_id)

        # Add new entry to tranactions table
        db.execute("INSERT INTO transactions (user_id, symbol, shares, price, transaction_time) VALUES (?,?,?,?,?)", user_id, stock["symbol"], shares, stock["price"], datetime.now())

        flash(f"Bought {stock['name']} stock.")

        return redirect("/")

    else:
        # GET method
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    transactions = db.execute("SELECT symbol, shares, price, transaction_time FROM transactions WHERE user_id = ? ORDER BY transaction_time DESC", session["user_id"])
    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 400)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":

        # Look up symbol
        stock = lookup(request.form.get("symbol"))

        if stock == None:
            return apology("symbol not found", 400)
        else:
            # Display quoted symbol
            return render_template("quoted.html", stock=stock)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Assign form data to variables
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # Ensure username was submitted
        if not username:
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not password:
            return apology("must provide password", 400)

        # Ensure password confirmation was submitted
        elif not confirmation:
            return apology("must confirm password", 400)

        # Ensure password and password confirmation match
        elif password != confirmation:
            return apology("passwords do not match", 400)

        # Generate hash
        hash = generate_password_hash(password, method="pbkdf2:sha1", salt_length=8)

        try:
            # Add user to database
            new_user = db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", username, hash)
        except:
            return apology("username already exists")

        session["user_id"] = new_user

        return redirect("/")

    else:
        return render_template("register.html")



@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    user_id = session["user_id"]

    if request.method == "POST":

        # Look up symbol
        stock = lookup(request.form.get("symbol"))
        shares = int(request.form.get("shares"))

        if stock == None:
            return apology("must select a symbol", 400)

        if shares < 0:
            return apology("positive values only", 400)

        # Get the number of shares owned by the user
        user_shares = db.execute("SELECT SUM(shares) AS shares FROM transactions WHERE user_id = ? AND symbol = ?", user_id, stock["symbol"])[0]["shares"]

        if user_shares < shares:
            return apology("not enough shares", 400)

        # Calculate transaction value and user's cash balance
        transaction_value = shares * stock["price"]
        user_id = session["user_id"]
        cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]

        # Update cash balance in users table
        new_cash_balance = cash + transaction_value
        db.execute("UPDATE users SET cash = ? WHERE id = ?", new_cash_balance, user_id)

        # Add new entry to tranactions table
        db.execute("INSERT INTO transactions (user_id, symbol, shares, price, transaction_time) VALUES (?,?,?,?,?)", user_id, stock["symbol"], (-1)*shares, stock["price"], datetime.now())

        flash(f"Sold {stock['name']} stock.")

        return redirect("/")

    else:
        # If method is GET pass an array of the current user's stocks to the sell template
        stocks = db.execute("SELECT symbol FROM transactions WHERE user_id = ? GROUP BY symbol HAVING SUM(shares) > 0", user_id)
        return render_template("sell.html", stocks = [row["symbol"] for row in stocks])


@app.route("/settings", methods=["GET", "POST"])
@login_required
def settings():
    """Allow user to change password"""
    if request.method == "POST":

        # Assign form data to variables
        user_id = session["user_id"]
        password = request.form.get("password")
        new_password = request.form.get("new_password")
        confirmation = request.form.get("confirmation")

        # Ensure current password was submitted
        if not password:
            return apology("must provide current password", 400)

        # Ensure password was submitted
        elif not new_password:
            return apology("must provide new password", 400)

        # Ensure password confirmation was submitted
        elif not confirmation:
            return apology("must confirm new password", 400)

        # Ensure password and password confirmation match
        elif new_password != confirmation:
            return apology("passwords do not match", 400)

        # Query database for current password
        rows = db.execute("SELECT * FROM users WHERE id = ?", user_id)

        # Ensure username exists and password is correct
        if not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("incorrect password", 400)

        # Generate password hash and update database
        hash = generate_password_hash(new_password, method="pbkdf2:sha1", salt_length=8)
        db.execute("UPDATE users SET hash = ? WHERE id = ?", hash, user_id)

        # Display message
        flash("Password updated")

        return redirect("/settings")

    else:
        return render_template("settings.html")
