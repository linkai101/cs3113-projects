# Volley Pong
For CS-UY 3113's Project 2. Pong-inspired volleyball game.

## Known issues
- Instances of `Entity` type are double-initialized, with `init()` called in the constructor and again in the game logic code within the `Game` class. Functionality works as expected, however.