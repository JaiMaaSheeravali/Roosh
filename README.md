**To execute the shell:**
make
./output/main

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
 </ol>
 
 <h3>Remaining</h3>
  <ol>
   <li><s>History command left</s></li>
   <li>Pipes and redirections are not yet supported</li>
   <li><s>Extra arguments are not checked sometimes</s></li>
  </ol>
