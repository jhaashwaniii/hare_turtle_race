# hare_turtle_race
Write a few cooperating threads:

I. Turtle

II. Hare

III. Randomizer

IV. Reporter


As we know, since our childhood, a hare and a turtle went for a race, and the turtle won!! ​ Why?
Maybe luck worked out for the turtle. Now, let’s recreate the scene and assert that it is random
luck (Randomizer) that decides the winner.
Input specification
1. Time unit intervals in which Reporter must report race standings (Current hare and turtle
distance)
2. Turtle speed: Distance turtle moves every unit time
3. Hare speed: Distance hare moves every unit time
4. Hare sleep time: Time units for which hare rests after moving once
(As in the story! The hare rests after every time it moves)
5. Minimum distance between hare and turtle when hare panics and moves
(Remember! It can only move after it has finished resting)
6. Finish distance
7. Number of times Randomizer intervenes/makes an impact (N)
8. N ​ lines of “player, time and distance” specifying N interventions of the randomizer
a. Player: Decide whether to move hare or turtle
b. Time: time unit at which this particular intervention must happen
c. Distance: The distance by which the player has to be displaced (can be negative)
Pseudocode for the five threads involved are as follows:
Turtle () {
while (no_winner) {
Move at the specified speed every unit time}
}
Hare () {
while (no_winner)
if (turtle is far off)
Sleep (for the specified time units);
else
Move at the specified speed for a single time unit
Sleep (for the specified time units)
}
Randomizer () {
while (no_winner)
if (time_to_reposition)
Select hare or turtle and reposition;
}
Reporter () {
while (race is on) {
Report the positions of hare and turtle in distance from the start in specific time
unit intervals given in input;
} }
Init () {
Create 4 parallel threads and run each of the four threads (Hare / Turtle / Randomizer
and Reporter.
while (no_winner)
Synchronise all four threads
return winner and reap all threads;
}
Note: ​ If hare and turtle cross the finish line at the same time unit then we’ll let the turtle win,
because slow and steady wins the race, in stories and in assignments!
Randomizer must intervene and reposition a player at ​ the beginning of a particular time unit
(ie when it's the randomizer’s turn to reposition)​ .
