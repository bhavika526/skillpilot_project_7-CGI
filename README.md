# C++ CGI Greeting Program

A small web project that receives a name from an HTML form and shows a personalized greeting. The greeting is created by a **C++ CGI program** that runs on an Apache web server.

## How it works

1. The user opens `index.html` and types a name in the form.
2. The form sends the name to the server using the **query string** (GET method), for example: `/cgi-bin/greeting.exe?name=Bhavika`
3. Apache runs the C++ program and passes the query string in the `QUERY_STRING` environment variable.
4. The C++ program reads the query string, extracts the name, and prints an HTML page.
5. The browser shows: **Hello, Bhavika!**

## Task Steps

| Step | Where it is done |
|---|---|
| 1. Design an HTML form with a text input for the user's name | `index.html` |
| 2. Write a C++ CGI program that reads the query string, extracts the name, and outputs HTML | `cgi-bin/greeting.cpp` |
| 3. Configure the web server to execute the CGI script and test the interaction | `apache/httpd-cgi.conf` and the steps below |

## Project Structure

```
cgi-greeting/
├── index.html              (HTML form with a text input for the name)
├── cgi-bin/
│   └── greeting.cpp        (C++ CGI program)
├── apache/
│   └── httpd-cgi.conf      (web server configuration)
└── README.md
```

## Features

- HTML form with a labeled text input, a `required` check, and a 50 character limit
- Reads the `QUERY_STRING` environment variable with `getenv`
- Decodes URL-encoded text (`+` becomes a space, `%21` becomes `!`)
- Finds the `name` value even when other parameters are present
- Escapes HTML characters, so a name like `<script>` is shown as text and is not executed
- Shows a friendly message when no name is sent
- Works with names in other languages (UTF-8)

## Requirements

- A C++ compiler (g++), for example MinGW-w64 on Windows
- Apache web server, for example [XAMPP](https://www.apachefriends.org)

## Step 1: Compile the program

**Windows (Git Bash or Command Prompt):**
```
cd cgi-bin
g++ greeting.cpp -o greeting.exe -static
```
The `-static` option puts the needed libraries inside the file, so Apache can run it without missing DLL errors.

**Linux / macOS:**
```
g++ cgi-bin/greeting.cpp -o greeting.cgi
chmod +x greeting.cgi
```
For Linux, change the form action in `index.html` to `/cgi-bin/greeting.cgi`.

## Step 2: Test the program without a server (optional)

**Git Bash:**
```
QUERY_STRING="name=Bhavika" ./greeting.exe
```

**Command Prompt:**
```
set QUERY_STRING=name=Bhavika
greeting.exe
```

The output starts with `Content-Type: text/html` and contains `<h1>Hello, Bhavika!</h1>`.

## Step 3: Configure Apache (XAMPP)

1. Copy `greeting.exe` to `C:\xampp\cgi-bin\`.
2. Copy `index.html` to `C:\xampp\htdocs\`.
3. Open `C:\xampp\apache\conf\httpd.conf` and make sure it contains the settings from `apache/httpd-cgi.conf`:
   - `ScriptAlias /cgi-bin/ "C:/xampp/cgi-bin/"`
   - `Options +ExecCGI` for the `cgi-bin` directory

   XAMPP usually has these settings already.
4. Open the XAMPP Control Panel and click **Start** next to **Apache**. If you changed `httpd.conf`, click **Stop** and **Start** again.

## Step 4: Test the interaction

1. Open `http://localhost/index.html` in a browser.
2. Type a name, for example `Bhavika`, and click **Greet Me**.
3. The browser opens `http://localhost/cgi-bin/greeting.exe?name=Bhavika` and shows **Hello, Bhavika!**

You can also open that address directly and change the name in the URL.

## Troubleshooting

| Problem | Fix |
|---|---|
| 500 Internal Server Error | Compile again with `-static` and copy the new `greeting.exe` to `C:\xampp\cgi-bin\` |
| 404 Not Found | Check that `greeting.exe` is inside `C:\xampp\cgi-bin\` |
| The browser shows the code or downloads the file | `ExecCGI` is not enabled. Check `httpd.conf` and restart Apache |
| "No name received" | The name was empty. Type a name in the form |

## Technologies Used

- C++
- HTML5, CSS3
- CGI (Common Gateway Interface)
- Apache HTTP Server

