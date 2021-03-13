-- Write a SQL query to list the names of all people who starred in a movie in which Kevin Bacon also starred.
-- Your query should output a table with a single column for the name of each person.
-- There may be multiple people named Kevin Bacon in the database. Be sure to only select the Kevin Bacon born in 1958.
-- Kevin Bacon himself should not be included in the resulting list.
SELECT DISTINCT(name)
FROM
(
    SELECT movies.id, movies.title
    FROM movies
    JOIN stars
    ON movies.id = stars.movie_id
    JOIN people
    ON stars.person_id = people.id
    WHERE name = "Kevin Bacon"
    AND birth = 1958
) as filter
JOIN stars
ON filter.id = stars.movie_id
JOIN people
ON stars.person_id = people.id
WHERE NOT people.name = "Kevin Bacon";