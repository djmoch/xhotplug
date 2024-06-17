# xhotplug

X11 monitor attach/detach event responder

## Installation

Install must run as root:

``` sh
make
make install
```

## Usage

There are several ways to make use of xhotplug. The easiest is to
add the following line to $HOME/.xession (or $HOME/.xinitrc if you
use xinit):

```
xhotplug script &
```

Many desktop environments provide a way to add startup programs,
although they probably also detect new displays on their own.

## Verifying Release Archives

Archives created prior to December 1, 2022 can be verified in one of
two ways:

1. Each .tar.gz archive is signed inline with signify(1) and can be
   verified (using the -z flag) against the public key located at
   https://www.danielmoch.com/static/djmoch-signify.pub

2. Using the .sig file corresponding to an archive, verification can
   be done with gpg(1) against the public key located at
   https://www.danielmoch.com/static/gpg.asc

Archives created on or after December 1, 2022 will only be signed with
`signify`, and not with `gpg`.
To support the use of `minisign`, which is compatable with `signify`,
detached signatures will be created.
These signature files are indicated with a `.minisig` extension.

## Documentation

``` sh
man xhotplug
```

## License Terms

See the LICENSE file in this repository
