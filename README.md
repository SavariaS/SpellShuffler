# About

SpellShuffler is an Elden Ring mod that equips a random spell after each spell cast. It is inspired by thefifthmatt's [Spell Randomizer](https://www.nexusmods.com/eldenring/mods/6962).

# Configuration

The mod is configurable through the SpellShuffler.ini file. This file must be in the same folder as SpellShuffler.dll and is loaded only once when the game starts. The options are:
- allowSpellSwitching: When `false`, the player cannot switch between equipped spells
- equippedSpellsOnly: When `true`, the mod will only consider memorized spells. When `false`, the mod will consider every spell in the game, as long as the player meets the requirements for them.
- sorceriesOnly: When `true`, the mod will only equip sorceries.
- incantsOnly: When `true`, the mod will only equip incantations.

# Building

```
mkdir build && cd build
cmake ..
cmake --build . --config Release
```

# Dependencies

The dependencies are included in the project. William Tremblay, aka '[tremwil](https://github.com/tremwil)' gave me permission to use the files he wrote for The Grand Archive's Elden Ring cheat table.

- [RTTIHook](https://github.com/Dasaav-dsv/RTTIHook)
- [Pattern16](https://github.com/Dasaav-dsv/Pattern16)
- [mINI](https://github.com/metayeti/mINI)
- [Elden-Ring-CT-TGA](https://github.com/The-Grand-Archives/Elden-Ring-CT-TGA)