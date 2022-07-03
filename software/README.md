# Flower power code
This code is generated with [cortex-m-quickstart](https://github.com/rust-embedded/cortex-m-quickstart)

> A template for building applications for ARM Cortex-M microcontrollers

## Dependencies

To build this embedded program you'll need:

- Rust 1.31, 1.30-beta, nightly-2018-09-13 or a newer toolchain. e.g. `rustup
  default beta`

- The `cargo generate` subcommand. [Installation
  instructions](https://github.com/ashleygwilliams/cargo-generate#installation).

- `cargo flash` subcommand. Install it with `cargo install cargo-flash`
- `rust-std` components (pre-compiled `core` crate) for the ARM Cortex-M
  targets. Run:
 
``` console
$ rustup target add thumbv6m-none-eabi
```

## How to build and flash

**NOTE**: This is the very short version that only covers building programs. For
the long version, which additionally covers flashing, running and debugging
programs, check [the embedded Rust book][book].

[book]: https://rust-embedded.github.io/book

1. Build the debug version of application.

``` console
$ cargo build
```

or release version

``` console
$ cargo build --release
```

2. Flash firmware
   * Connect ST Link device to USB port
   * Flash firmware to STM32F030C6T6 microcontroller
     ``` console
     cargo flash --release --chip STM32F030C6Tx 
     ```

# License

This template is licensed under either of

- Apache License, Version 2.0 ([LICENSE-APACHE](LICENSE-APACHE) or
  http://www.apache.org/licenses/LICENSE-2.0)

- MIT license ([LICENSE-MIT](LICENSE-MIT) or http://opensource.org/licenses/MIT)

at your option.

## Contribution

Unless you explicitly state otherwise, any contribution intentionally submitted
for inclusion in the work by you, as defined in the Apache-2.0 license, shall be
dual licensed as above, without any additional terms or conditions.

## Code of Conduct

Contribution to this crate is organized under the terms of the [Rust Code of
Conduct][CoC], the maintainer of this crate, the [Cortex-M team][team], promises
to intervene to uphold that code of conduct.

[CoC]: https://www.rust-lang.org/policies/code-of-conduct
[team]: https://github.com/rust-embedded/wg#the-cortex-m-team
