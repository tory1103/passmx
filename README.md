# PASSMX - Password mixer
Passmx is a password generator that mixes a secret and a passphrase to generate it. It's NOT a password manager or password store, it's just a password generator.

## Features
- Password generation
- Custom password length
- Verbose output

## Documentation
- [Installation](./docs/install.md)
- [Usage](#usage)

# Usage
```bash
# Show usage menu
./passmx -h

# Generate a password of 25 character length
# The passphrase can be any string,
# but its highly recommended to use some easy to remember phrase like shown below
./passmx -s "mysecretkey" \
         -p "i registered in google.com with username@gmail.com" \
         -l 25 -v

# This example shows that you can use any string to generate passwords
./passmx -s "mysecretkey" \
         -p "__01234//5\\6789__" \
         -l 8 -v
```