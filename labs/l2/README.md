## Non-programming tasks.

### 2.1 Minix 3

1. We use pkgin to update, install, and remove packages. 
```bash
pkgin update
pkgin install clang
pkgin -r remove clang
```

2. 
ifconfig: show the network interface configuration

adduser: create a new user to the system

passwd: change the password of a user

3.
```bash
vi /usr/pkg/etc/pkgin/repositories.conf
# Add the following to the file and comment the line of ftp://
# ftp://ftp.minix3.org/pkgsrc/packages/$osrelease/$arch/All
http://www.minix3.org/pkgsrc/packages/$osrelease/$arch/All
http://focs.ji.sjtu.edu.cn/minix
```

### 2.2

1. Continuous Integration/Continuous Delivery (CI/CD): CI means integrating code changes from multiple contributors into a shared repository. CD means automating the entire software delivery process, from building and testing to staging and deployment. By CI/CD, we can ensure that the codebase remains in a working state and reduce the time and risk associated with releasing software.

2. It can:
    1. Increase readability and maintainability of the codes.
    2. Friendly for collaboration
    3. Reduce the cognitive load of developers
    4. Reduce bugs
    5. Speed up the code review process
    6. Simplify the onboarding process
    7. Maintain codebase uniformity
    8. Aid in code refactoring
    9. Help tools and automation
    10. Reflect professionalism
    11. Ensure the longevity and sustainability of a codebase

3. Sanitizers are benefitial in many aspects, including:
    1. Early Bug Detection
    2. Memory Safety
    3. Undefined Behavior Detection
    4. Crash Reports
    5. Reduced Debugging Effort
    6. Improved Code Quality
    7. Security Enhancement
    8. Cross-Platform Compatibility
    9. Ease of Integration
    10. Open Source Community

### 3

1. setup SSH and change port. The default SSH port is 22.
```bash
pkgin update
pkgin install openssh
vi /usr/pkg/etc/ssh/sshd_config # add Port 2222
# On own computer
ssh root@192.168.133.128 -p 2222
```

2. In ~/.ssh directory:

    1. authorized_keys: contains the public keys of the users that are allowed to log in to the system.
    2. id_ed25519: the private key of the user.
    3. id_ed25519.pub: the public key of the user.
    4. known_hosts: contains the public keys of the hosts that the user has connected to.
    5. config: contains the configuration of the SSH client.
    6. known_hosts.old: the backup of known_hosts.

Create an Entry:
```text
Host    minix
        HostName 192.168.133.128
        Port     2222
        User     myname
```

3. SSH relies on cryptographic key pairs (usually RSA, DSA, or ECDSA) where one part is private and securely stored on the user's device, while the other is shared with the remote server by adding it to the `~/.ssh/authorized_keys` file. During connection, the user's machine sends its public key to the server, which checks if it matches any entry in the `authorized_keys` file. If a match is found, the server challenges the user to prove possession of the private key, and the user uses it to sign a challenge, which the server verifies with the stored public key for authentication.

Generate keys and log in:
```bash
chmod 700 ~/.ssh
ssh-keygen -t rsa
ssh-copy-id -i ~/.ssh/id_rsa myname@192.168.133.128 -p 2222
```

### 4 Basic git

Git command usage:
1. help: display help information about Git
2. init: initialize a git repository
3. checkout: switch branches or restore working tree files
4. branch: list, create, or delete branches
5. push: update remote refs along with associated objects
6. pull: fetch from and integrate with another repository or a local branch
7. merge: join two or more development histories together
8. add: add file contents to the index
9. diff: show changes between commits, commit and working tree, etc
10. tag: create, list, delete or verify a tag object signed with GPG
11. log: show commit logs
12. fetch: download objects and refs from another repository
13. commit: record changes to the repository
14. clone: clone a repository into a new directory
15. reset: reset current HEAD to the specified state