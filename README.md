**To execute the shell:**

```shell
cmake .
make
./output/main
```

c++17 required

**Theory:**

- The basic shell perform these basic operations:

  1. Load config files
  2. Run REPL loop
  3. Clean up memory to prevent leakage.

- In the Loop part following operations are performed:
  1. Read the commands entered by the user.
  2. It will parse the commands so computer can understand the commands.
  3. Execute the commands.

<h2>Core Features:</h2>

<h3>Implemented</h3>
 <ol>
    <li>All Linux Commands are supported (tell if some commands doesn't work)</li>
    <li>Batch Mode done (Use "rsh filename.rsh" \\ Only .rsh files allowed)</li>
    <li>Git Branch, Username, PC name and current directory display during input prompt.</li>
    <li>"exit" can be used to exit the shell and revert to default linux shell</li>
    <li>pipes ('|') and redirections ('<' or '>' or '>>') supported</li>
    <li>Google a word for its meaning in more than 6 languages</li>
 </ol>
 
 <h3>Remaining</h3>
  <ol>
   <li>Aliases not yet implemented.</li>
   <li>cd is not fully functional.</li>
  </ol>

**Errors and Solutions:**

1. If cmake . fails with message :
   `"Could NOT find OpenSSL, try to set the path to OpenSSL root folder in the"`,

   then run command

   `sudo apt-get install libssl-dev`
   on your terminal(tested for ubuntu 18.04 LTS)
