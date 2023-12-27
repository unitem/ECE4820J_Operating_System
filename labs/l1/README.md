# ECE482 Lab 1 Tasks

## Author

Name: Weiqing Xu

Student ID: 520021910400

### mkdir, touch, mv, cp, and ls commands

```bash
touch test
mkdir dir
mv test dir/test.txt
cp dir/test.txt dir/test_copy.txt
ls -al dir
```
### grep command

```bash
grep -rl "127.0.0.1" /etc
```

It shows the result "/etc/security/access.conf /etc/dhcp/dhclient.conf /etc/hosts"

For the second one, "xwq" is my username, so I should run

```bash
grep -E 'xwq.*root|root.*xwq' /etc/passwd
```
### find command

```bash
find /etc -type f -atime -1
find /etc -type f -name '*netw*'
```

### man-page

```bash
man bash
/redirection
```
```bash
> # redirect standard output to a file (create if not exist)
>> # redirect standard output and append it to a file (create if not exist)
<<< # feed a string as input to a command
>&1 # duplicate file descriptor 1 (stdout)
2>&1 > # first redirects file descriptor 2 (stderr) to the same location as file descriptor 1 (stdout), and then redirects stdout to a file
```

tee command is used to read from standard input and write to standard output and files
```bash
command | tee [OPTION]... [FILE]...
```

### xargs and |

xargs command is used to build and execute command lines from standard input.
```bash
generating-some-input | xargs [options] [command]
```

pipe operator `|` is used to send the standard output (stdout) of one command as the standard input (stdin) to another command.
```bash
commandA | commandB
```

### head and tail

head/tail command is used to display the first/last few lines of a file, using `-n` option to specify the number of lines, the default is 10.

To “live display” a file as new lines are appended, use `tail -f [file]` command.

### ps, top, free, vmstat

1. `ps` is used to display information about running processes.
     ```bash
     ps aux # list all processes running on the system
     ps -u username # list all processes owned by a specific user
     ```

2. `top` provides a dynamic, real-time view of system processes and resource usage.

3. `free` is used to display information about system memory (RAM) usage. Use `-h` option to display the output in a human-readable format.

4. `vmstat` reports information about system memory, CPU usage, processes, and more. Use `-s` option to display a summary of statistics, and `-d` option to display disk statistics. Use `n` delay option to set the delay between updates in seconds. 

### sh, bash, csh, and zsh

1. `sh` is the original Unix shell, default shell on many Unix systems, lightweight and minimalistic, and has limited interactive features and scripting capabilities.
2. `bash` is the GNU Project's shell. It is an sh-compatible shell that provides extensive scripting capabilities and interactive features.
3. `csh` is a Unix shell featuring a C-like syntax.
4. `zsh` is a more feature-rich shell compared to bash, and has robust customization options. MacOS uses bash for compatibility but is moving towards zsh.

### `$0`, `$1`, `$?`, `$!`

1. `$0` is the name of the shell or shell script.
2. `$1` is the first command-line argument passed to the shell script.
3. `$?` is the exit status of the last executed command.
4. `$!` is the process ID of the most recently executed background (asynchronous) command.

### PS3 variable

`PS3` is the prompt used by the select command to prompt the user to select a choice from a list of options.

```bash
#!/bin/bash

PS3="Choose one option: "
options=("Option A" "Option B" "Option C" "Quit")

select choice in "${options[@]}"; do
    case $choice in
        "Option A")
            echo "You chose Option A"
            ;;
        "Option B")
            echo "You chose Option B"
            ;;
        "Option C")
            echo "You chose Option C"
            ;;
        "Quit")
            echo "Exiting..."
            break
            ;;
        *)
            echo "Invalid option"
            ;;
    esac
done

```

The code is /PS3.sh

### iconv

`iconv` is a command-line program and a standardized application programming interface (API) used to convert between different character encodings. It's useful to make files be properly displayed or processed by different applications, languages and locales.

```bash
iconv -f utf-8 -t utf-16le 1.txt > 2.txt
```

### $temp

1. `${#temp}` is the length of the variable temp.
2. `${temp%%word}` is to remove the longest match of pattern "word" from the value of temp.
3. `${temp/pattern/string}` is to replace the first match of pattern "pattern" with "string" in the value of temp.

### files in Unix like system

– / - root directory
– /bin - essential binary executables
– /boot - boot process files, including the Linux kernel (vmlinuz) and the initial RAM disk (initrd.img) used during system boot
– /etc - system-wide configuration files
– /lib - system shared libraries
– /mnt - mount point for a temporarily mounting external devices or filesystems
– /usr/bin - binary executables for user-level commands and applications
– /usr/share - architecture-independent data files shared among various applications and packages
– /usr/lib - libraries used by programs in /usr/bin and /usr/sbin
– /usr/src - source code, e.g. the Linux kernel source code
– /proc - virtual filesystem that provides information about running processes and system settings
– /sys -  virtual filesystem used to configure and interact with kernel parameters and settings
– /srv - data files for specific services provided by the system
– /media - mount point for removable media such as USB drives and external hard disks
– /opt - optional software or additional packages
– /var - variable data, such as logs, databases, and temporary files
– /sbin - system administration binaries used for system maintenance and recovery
– /dev - special files representing devices on the system, including hardware devices and virtual devices
– /vmlinuz - Linux kernel image file
– /initrd.img - initial RAM disk image used during the boot process, which contains necessary modules and drivers

### Game Script

The code below is in number.sh

```bash
#!/bin/bash

echo "This is a game to guess the randomly generated number."
echo "The random number is between 1 and 100."
echo "Generating..."

# Generate a random number between 1 and 100
random_number=$((RANDOM % 100 + 1))
# echo $random_number
echo "Successfully generated."

# Loop until the user guesses the correct number
while true; do
    read -p "Enter your guess: " user_number
    # echo $user_number

    # Check if valid
    # =~ for regex matching, [[]] for conditional expression, ^[0-9]+$ for matching numbers, "" for treating user_number as string
    if ! [[ "$user_number" =~ ^[0-9]+$ ]]; then
        echo "Invalid number. Try again."
        continue
    fi

    # Compare the user's guess with the random number
    if [ $user_number -lt $random_number ]; then
        echo "Try bigger number."
    elif [ $user_number -gt $random_number ]; then
        echo "Try smaller number."
    else
        echo "Congratulations! You guessed the correct number."
        break
    fi
done

```



