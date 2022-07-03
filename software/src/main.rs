#![no_main]
#![no_std]

use panic_halt as _;

use stm32f0xx_hal::{pac, prelude::*};
use cortex_m::{peripheral::syst::SystClkSource::Core};
use cortex_m_rt::{entry};
use cortex_m::delay::Delay;
use stm32f0xx_hal::adc::AdcSampleTime;
use stm32f0xx_hal::rcc::HSEBypassMode::NotBypassed;


#[entry]
fn main() -> ! {
    if let (Some(p), Some(cp)) = (
        pac::Peripherals::take(),
        cortex_m::peripheral::Peripherals::take(),
    ) {
        cortex_m::interrupt::free(move |cs| {

            let mut flash = p.FLASH;
            //let mut rcc = p.RCC.configure().sysclk(16.mhz()).freeze(&mut flash);
            let mut rcc = p.RCC.configure().hse(16.mhz(), NotBypassed).sysclk(48.mhz()).freeze(&mut flash);

            let gpioa = p.GPIOA.split(&mut rcc);
            let gpiob = p.GPIOB.split(&mut rcc);

            let mut syst = cp.SYST;

            // Set source for SysTick counter, here full operating frequency (== 8MHz)
            syst.set_clock_source(Core);

            // Set reload value, i.e. timer delay 8 MHz/counts
            syst.set_reload(16_000_000 - 1);

            // Start SysTick counter
            syst.enable_counter();

            // Start SysTick interrupt generation
            syst.enable_interrupt();

            // USART1 at PA9 (TX) and PA10(RX)
            let mut a0 = gpioa.pa0.into_analog(cs);
            let mut a1 = gpioa.pa1.into_analog(cs);
            let mut a2 = gpioa.pa2.into_analog(cs);
            let mut wp1 = gpiob.pb14.into_push_pull_output(cs);
            let mut wp2 = gpiob.pb13.into_push_pull_output(cs);
            let mut wp3 = gpiob.pb12.into_push_pull_output(cs);
            let mut delay = Delay::new(syst, 16_000_000);

            // Initialise ADC
            let mut adc = stm32f0xx_hal::adc::Adc::new(p.ADC, &mut rcc);
            adc.set_sample_time(AdcSampleTime::T_239);
            loop {
                if adc.read_abs_mv(&mut a0) > 2000 {
                    wp1.set_high().ok();
                    while adc.read_abs_mv(&mut a0) > 2000 {
                        delay.delay_ms(100);
                    }
                    wp1.set_low().ok();
                }
                if adc.read_abs_mv(&mut a1) > 2000 {
                    wp2.set_high().ok();
                    while adc.read_abs_mv(&mut a1) > 2000 {
                        delay.delay_ms(100);
                    }
                    wp2.set_low().ok();
                }
                if adc.read_abs_mv(&mut a2) > 2000 {
                    wp3.set_high().ok();
                    while adc.read_abs_mv(&mut a2) > 2000 {
                        delay.delay_ms(100);
                    }
                    wp3.set_low().ok();
                }
                delay.delay_ms(3600000);
            }
        });
    }
    loop {
        continue;
    }
}

