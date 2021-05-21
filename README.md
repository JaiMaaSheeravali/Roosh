
**To build and run the shell:**

```shell
sudo apt-get update
sudo apt-get install cmake
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
    <li>All Linux Commands are supported.</li>
    <li>Batch Mode done (Use "rsh filename.rsh" \\ Only .rsh files allowed)</li>
    <li>Git Branch, Username, PC name and current directory display during input prompt.</li>
    <li>"exit" can be used to exit the shell and revert to default linux shell.</li>
    <li>pipes ('|') and redirections ('<' or '>' or '>>') supported.</li>
    <li>"history" command can be used to list all the commands used in current shell session.</li>
 </ol>
 
 <h3>Remaining</h3>
  <ol>
   <li>Aliases not yet implemented.</li>
   <li>Commands containing string (" ") may not function properly.</li>
  </ol>
  
 <h3>Additional Features</h3>
 
 <h3>Implemented</h3>
  <ol>
    <li>Basic Text editing while entering command is supported.</li>
    <li>Command History access using &#8593 and &#8595 arrow key.(Limited to current shell session only)</li>
    <li>Tab Auto-Completion during input is supported.</li>
    <li>Basic Arithmetic Operation can be performed using "calc [expresssion]".<br />  (Here expression is any mathematical expression without a space)  </li>
    <li>"google <word_to_search> <optional:lang_code>" can be used to google meaning of any word.<br />
        (Note: An active Internet Connection is required to run this command.)</li>
  </ol>
      
 <h3>Beginner Shell Tutorial</h3>
 
 <h3>Implemented</h3>
  <ol>
    <li>"tutorial" command be used to run tutorial content and learn about basic shell and git commands inside shell.</li>
    <li>"tutorial level [level-number]" can be used to access practice levels and find the hidden flag using shell commands.</li>
    <li>"tutorial level [level-number] password" can be used to submit the hidden flag in grading software to pass that level.</li>
  </ol>

**Errors and Solutions:**

1. If cmake . fails with message :
   `"Could NOT find OpenSSL, try to set the path to OpenSSL root folder in the"`,

   then run command

   `sudo apt-get install libssl-dev`
   on your terminal(tested for ubuntu 18.04 LTS)
