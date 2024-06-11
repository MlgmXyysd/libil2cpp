# libil2cpp

Collection of source code for Unity IL2CPP

All code in [libil2cpp](libil2cpp) is dumped directly from the Unity installer (Windows)

## Version Reference

List of IL2CPP versions corresponding to Unity versions

- [Version Reference](IL2CPP_Version.md)

## How to determine Unity version?

In Unity products, there are numerous resource files, including Unity engine default resources and custom resources added by developers. Default resources are typically not encrypted, but custom resources may be. These files contain information about the version of Unity used to build the project.

To obtain this information, read the file using a hex editor (e.g. `010 Editor` or `ImHex`). In the following example, we read the `unity default resources` file. Near the beginning of the file, `2021.3.20f1` indicates the Unity version. According to [Version Reference](#version-reference), the corresponding IL2CPP version is `29`.

### Example

- Part of file `unity default resources`

```
00000000  00 00 00 00 00 00 00 00 00 00 00 16 00 00 00 00  |................|
00000010  00 00 00 00 00 00 09 09 00 00 00 00 00 40 6C D8  |.............@l.|
00000020  00 00 00 00 00 00 10 00 00 00 00 00 00 00 00 00  |................|
00000030  32 30 32 31 2E 33 2E 32 30 66 31 00 0D 00 00 00  |2021.3.20f1.....|
00000040  00 09 00 00 00 73 00 00 00 00 FF FF F4 6E 8E 30  |.....s.......n.0|
```

## Anecdote

- Unity 4.6.2f1 introduced IL2CPP, but only for the iOS platform.
- Unity 5.2.0f2 included experimental IL2CPP support for other platforms, which became formally supported in Unity 5.3.0f4.
- The IL2CPP header file was added in Unity 4.6.5f1.
- The IL2CPP source code appeared in Unity 4.6.9f1 but was removed in Unity 5.0.1p1.
- After Unity 5.2.1p3 (except for Unity 5.3.0b1), the IL2CPP source code has always been present.
- The first public version of IL2CPP was `16`.
- In Unity, both IL2CPP and Mono use `global-metadata.dat`. IL2CPP refers to it as GlobalMetadata, while Mono calls it GlobalMonoMetadata, with a version of `1`.
- The `IL2CPP_ASSERT` function (formerly `assert`) for validating the header contents of `global-metadata.dat` does not work in non-debug builds.

## Also see

- [Il2CppMemoryDumper](https://github.com/MlgmXyysd/Il2CppMemoryDumper)
- [Il2CppDumper-Standalone](https://github.com/MlgmXyysd/Il2CppDumper-Standalone)

## Credits

- [Unity Technologies](https://unity.com/)
- [Jaida Wu (NekoYuzu)](https://github.com/MlgmXyysd)
- [Midori Kochiya](https://github.com/kmod-midori)
