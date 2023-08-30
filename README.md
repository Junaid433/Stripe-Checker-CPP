# Stripe-Checker-CPP
C++ Console Based Advanced Stripe CC Checker Script

You need your own secret keys from stripe to check cards.

## Pre-Requirements:
> A C++ Compiler (MSVC, CLANG AND GCC Recommended)
> Libcurl Library Installed

## Compile:
  ### Linux:
    ```
    gcc -o main main.cpp -lcurl -lstdc++ -pthread
    ```
  ### Windows:
    ```
    (Please use Visual Studio in Windows)
    cl -o main main.cpp -lcurl -lstdc++ -pthread
    ```
## Configuration:
```
(You must edit config.json as checker highly depends on it)
tg_id = Your telegram chat_id
bot = bot token
proxy = (Use this format: {http://ip:port}, {socks5://ip:port}) Example: http://69.69.69.69:8080
amount = 80 recommended. 1$ is 100
currency = any curreny supported by stripe but in lower chase
sk = your stripe secret key. use (\n) for multiple sk Example: sk_live_ok\nsk_live_notok
```

## Limitations:

```
Proxy Auth Not Supported. You can authorize your machine IP address instead!
If amount is <1 then charge output will be 0$
```

## Support:
 
  Telegram (https://t.me/fakehecker)

  Facebook (https://facebook.com/jnaid.rahman.im)

  Open issues on github

## Donation:

  BTC: ```35s7bwSiLocbs2wYg7HdsHihNbRSgq4DFi```

  LTC: ```MM8f9Gz3LCMAgavxuuoUtNWRDazsKmycKu```

  USDT [TRC20] : ```TDW1piifbkZe2c3nTDTZHKuJe2LU1NgzQV```

  TRX: ```TChp9kpxSxFuMsytLzhj4FtG74Ad8ggNy3```
