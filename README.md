
# 🧠 LanaBot – Smart Market-Making on Solana

![LanaBot Hero](https://user-images.githubusercontent.com/your-lanabot-gif.gif)

**LanaBot** is a reactive market-making bot on Solana built with Rust and Anchor. It uses a blend of sentiment and momentum-based algorithms to manage liquidity and earn passive yield.

## 🔥 Key Features

- 📊 Automated orderbook balancing
- 🤖 Smart market-making strategies
- 🔁 Supports Serum & OpenBook
- 🔐 Secure key management

## 📦 Built With

- Rust
- Solana
- Anchor
- WebSockets for real-time pricing

## 💻 Installation

```bash
git clone https://github.com/S4NTH174UN9/LanaBot.git

🔧 Configuration
Update config.toml:
[network]
rpc = "https://api.mainnet-beta.solana.com"

[bot]
maker_fee = 0.002
spread = 0.003
depth = 10
🎬 Bot in Action

Telegram: [@YourHandle]
Discord: yourusername#0000
Email: your@email.com
Twitter: @yourhandle
cd LanaBot
cargo build --release
