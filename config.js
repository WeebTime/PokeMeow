var config = {
  "token": "Token Here",
  "update_type": "long-polling", // or "webhook"
  "webhook": "https://www.example.com/<token>",
  "proxy": "http://127.0.0.1:8118",
  "bot_name": "example_bot", // check bot name,
  "private": false,
  "group_url": "https://telegram.me/groupnamehere",
  "admin_id": "admin id",
  "gba": {
    "rom_file": "/dankdel/to/game.gba",
    "savedata_file": "/dankdel/to/game.sav"
  }
};

module.exports = config;
