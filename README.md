# kof-xiv-input-frames

Display frame count aside from inputs inside KOF XIV (it works in beta at least, and it should be stable enough).

Uses EasyHook to inject inside the game.

## How to?

* Go to the release page, and get the latest release. 
* Extract the zip archive somewhere
* Launch your game
* Launch the provided exe

It should display something like "Library is injected, everything is alright.", type enter to close the window. You should see frames now!

## How?
The frame count is actually IN the game. I just force the game to draw it via dll injection.

What the dark magic lookup variable? Can't tell you.

## Is it safe?
I think so, nothing prevents code injection in the game so we're good.

Also, I don't modify the gameplay, just the input drawing function as you can see in the code.
