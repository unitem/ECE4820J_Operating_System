# Non-Programming Exercises

## 2.2

```bash
diff -u original.txt modified.txt > mypatch.patch # Use the command of diff and patch to edit a file and create a patch corresponding to the changes.
patch < mypatch.patch # apply the patch to the original file.
patch -R < mypatch.patch # reverse the patch.
```

## 3

```bash
ifconfig | awk '/inet / && !/127.0.0.1/ {print $2}' # The result is like '192.168.*.*'
```