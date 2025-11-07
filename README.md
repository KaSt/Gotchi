# Palnagotchi for M5 Cardputer

![Palnagotchi](https://github.com/viniciusbo/m5-palnagotchi/blob/master/palnagotchi.jpg?raw=true)

A friendly unit for those lonely Pwnagotchis out there. It's written to run on the M5Stack (tested on the Cardputer, the M5StickC Plus2 (might run also on the M5StickC (Plus) but untested) and the M5AtomS3(R)) and I'll try to add support to other M5 devices in the future.

I reverse engineered the Pwngrid advertisement protocol and made it possible for several M5Stack devices to advertise to the Pwngrid as a Pwnagotchi. All brain policy parameters that could negatively impact AI learning were removed from the advertisemenet data.

The Pwngrid works by sending Wifi beacon frames with a JSON serialized payload in Wifi AC headers, containing the Pwnagotchi's data (name, face, pwns, brain policy between others). That's how nearby Pwnagotchis can detect and also learn each other. By crafting a custom beacon frame, this app can appear as a Pwnagotchi to other Pwnagotchis.

## Usage

- Run the app to start advertisement.
- Button layouts:
  - Cardputer: ESC or m toggles the menu. Use arrow keys or tab to navigate and OK to select option. Esc or m to go back to main menu.
  - M5StickC Plus2: Long press M5 button to toggle menu. Use top and bottom keys to navigate and M5 button (short press) to select option.
  - AtomS3(R): Long press display to toggle menu. Use double/triple tap display to navigate and short press display to select option.
- Top bar shows UPS level and uptime.
- Bottom bar shows total friends made in this run, all time total friends between parenthesis (needs EEPROM) and signal strengh indicator of closest friend.
- Nearby pwnagotchis show all nearby units and its signal strength.
- Palnagotchi gets a random mood every minute or so.

## Configuration

Palnagotchi now supports configuration via WiFi Access Point mode:

1. **Enter Configuration Mode:**
   - Press the menu button (ESC/m on Cardputer)
   - Navigate to "Settings" and press OK
   - Select "WiFi Config (AP)" and press OK

2. **Connect to Configuration AP:**
   - Connect your phone/computer to WiFi network: `Palnagotchi-Config`
   - Password: `palnagotchi`

3. **Access Configuration Page:**
   - Open a web browser and go to: `http://192.168.4.1`
   - Configure the following settings:
     - **Device Name**: Change from "Palnagotchi" to your custom name
     - **Display Brightness**: Adjust screen brightness (0-255)
     - **Sound**: Enable or disable sound features

4. **Save and Exit:**
   - Click "Save Configuration" to store your settings
   - Press the menu button to exit AP mode
   - Settings will persist across reboots (stored in EEPROM)
   - The configuration AP will auto-timeout after 5 minutes

**Note:** Your custom device name will be used when advertising to the Pwngrid network, allowing other Pwnagotchis to see your personalized name!

## Why?

I don't like to see a sad Pwnagotchi.

## Planned features

- Friend spam?
