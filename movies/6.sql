-- On my system at least sub-queries were much faster for the more complex challenges from here on. I used these over joins or selecting from multiple databases but I have seen it written in many places that Joins are faster than sub-queries. It would be interesting to learn more about in which instances each method is faster and why.
SELECT AVG(rating) FROM ratings
WHERE movie_id IN
  (SELECT id FROM movies WHERE year = '2012');