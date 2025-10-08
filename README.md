# extended-client-message
A simple Pawn.RakNet handler to catch unexpected (or expected) SendClientMessage packets that are more than 144 characters.

### Requirements
[Pawn.RakNet v 1.6.0-omp](https://github.com/katursis/Pawn.RakNet/releases/tag/1.6.0-omp)  
[PawnPlus v 1.5.1](https://github.com/IS4Code/PawnPlus/releases/tag/v1.5.1)

## Instalation

Just drop `ecm.inc` into your includes folder, Make sure to `#include` it on your script AFTER including both required plugins.


## Usage

There isn't an action expected for you. This handler will intercept the SendClientMessage RPC, then check its length.  
If it doesn't exceed the SA:MP character limit (144 characters), it will continue with the common behavior.  
If not, it will be digested by a PawnPlus string, splitted into many lines and sent in batches to the client.  

This handler also supports color embedding in your string (so it will not do some weird cutoffs in your text color setup).

### Internal functions (that can be useful for extraordinary purposes)

`ecm_ReadString` is a wrapper for `BS_ReadString`, taking a PawnPlus `String:` as a valid output. It is used internally by this handler but can be useful in other cases.
```c
ecm_ReadString(BitStream:bs, String:output, offset, length)
```

## Important

Due to this include using some iterations and byte-per-byte reading (in order to read strings from packets into a PawnPlus string), it can be a little slow.  
However, a server with no load can handle reading an batch of 1000 BitStream packet string with 1300-ish characters, split and send them them right over in something about 400-500 miliseconds.  
It is recommended to exploit its capacities on special cases.

## To-do:

Esthetics:
- Check for spaces in the output text, then split the message taking in consideration the farest in-range space character.
