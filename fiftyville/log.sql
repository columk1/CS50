-- Keep a log of any SQL queries you execute as you solve the mystery.
-- Check tables.
.tables

-- Check crime_scene_reports table.
.schema crime_scene_reports

-- 1. Check crime scene reports using provided info.
SELECT description FROM crime_scene_reports
WHERE year = 2021
AND month = 7
AND day = 28
AND street = 'Humphrey Street';

-- CLUE: Crime happened at 10.15am. There were 3 witnesses who were interviewed that day.

-- 2. Read interview transcripts from that day.
SELECT name, transcript FROM interviews
WHERE year = 2021
AND month = 7
AND day = 28;

-- CLUE: Ruth saw thief drive away within 10 minutes of the theft
-- CLUE: Eugene recognized thief. Earlier that morning, before he arrived at Emma's bakery, he saw the thief there withdrawing some money from the ATM on Leggett Street.
-- CLUE: Raymond noticed thief on the phone for less than 1 minute as 'they' left the bakery. Planning to take earliest flight in the morning. Asked call receiver to buy ticket.

-- 3. Query people using the information from the interview transcripts.
-- Shows the name, phone number and passport number of anyone who meets our criteria based on what we know so far.
SELECT name, phone_number, passport_number FROM people
WHERE id IN
  (SELECT person_id FROM bank_accounts
   WHERE account_number IN
    (SELECT account_number FROM atm_transactions
    -- Made a withdrawal on Leggett Street that day
     WHERE year = 2021
     AND month = 7
     AND day = 28
     AND atm_location = 'Leggett Street'
     AND transaction_type = 'withdraw'))
AND phone_number IN
  (SELECT caller FROM phone_calls
  -- Made a call less than a minute long that day
   WHERE year = 2021
   AND month = 7
   AND day = 28
   AND duration < 60)
AND license_plate IN
-- Car was seen by bakery security cameras leaving the bakery shortly after the crime
  (SELECT license_plate FROM bakery_security_logs
   WHERE year = 2021
   AND month = 7
   AND day = 28
   AND hour = 10
   AND minute > 15
   AND minute < 30
   AND activity = 'exit');

-- SUSPECTS:
--+-------+----------------+-----------------+
--| name  |  phone_number  | passport_number |
--+-------+----------------+-----------------+
--| Diana | (770) 555-1861 | 3592750733      |
--| Bruce | (367) 555-5533 | 5773159633      |
--+-------+----------------+-----------------+

-- 4. Check the airport id of fiftyville airport.
SELECT * FROM airports
WHERE city = 'Fiftyville';

-- INFO: Fiftyville airport abbr. is 'CSF' and its id is 8.

-- 5. Check if Bruce or Diana have been on any flights out of Fiftyville.
-- Show their names and passports alongside the flight times, showing the destination city, ordered by date and time.
SELECT name, p.passport_number, f.day, f.hour, f.minute, a.city FROM people p
INNER JOIN passengers pas ON p.passport_number = pas.passport_number
INNER JOIN flights f ON pas.flight_id = f.id
INNER JOIN airports a ON f.destination_airport_id = a.id
WHERE name = 'Bruce'
OR name = 'Diana'
AND f.origin_airport_id = 8
ORDER BY f.day, f.hour;

-- SOLVED: Bruce was on the first flight out of Fiftyville in the morning of the 29th. He flew to New York.

-- 6. Check who Bruce called from the bakery during the theft.
SELECT name FROM people
JOIN phone_calls ph ON phone_number = ph.receiver
WHERE ph.year = 2021
AND ph.month = 7
AND ph.day = 28
AND ph.duration < 60 
AND ph.caller = '(367) 555-5533';

-- Robin was Bruce's accomplice.