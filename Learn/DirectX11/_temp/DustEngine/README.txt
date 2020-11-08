Dust Engine is a 2d top-down shooter game engine.
Code is licenced under GPL.
Project webpage: http://sourceforge.net/projects/dustengine/
Project author: Diamond_Dragon
Any responses mail to coffeecup.winner@gmail.com

Current release (0.1) is the first milestone release and have the following features (in presented demo):
- player movement (controls are WASD)
- player shooting (LMB)
- player weapon changing (keys 1, 2, 3)
- weapon element changing (RMB)
- enemies, minibosses and bosses spawning (every 1, 15 and 30 sec. respectively)
- enemies moving towards player
- score counter
- game pause (key P)
- game exit (Esc)
- game restart after game over (key R)

Elements system:
There are 4 elements: fire (orange-red), water (blue), air (light blue) and earth (brown). The neuntral element (or no element) have gray color.
There are two pairs of elements: fire-water and air-earth.
Hitting an enemy with the same element as your weapon's heal him with 0.5 the damage value.
Hitting it with the pair element (e.g. fire enemy with water weapon) damages him twice as damage value.
Hitting it with any other element (including gray) deals normal damage (= damage value).

The goal of the game demo is to kill as many enemies as you can.
The player dies when is touched by an enemy.
The enemy dies when its hit points are 0.

There are 3 weapons:
1 - blaster, 1 projectile, minimum damage, but penetrates enemies and damages every enemy that it have flown through and fires in a straight line, 1 shot in 0.3 sec
2 - shotgun, 10 wide-spreading projectiles, medium damage, deadly at melee range, 1 shot in 0.8 sec.
3 - assault rifle, 1 spreading projectile, maximum damage, 1 shot in 0.25 sec.

It's recommended to attack bosses with pair-elements (as explained above).

Known bugs:
Sometimes big enemies (bosses and minibosses) are spawn in other enemies position, and they all are standing in a place and not moving until the player kills that big enemy.