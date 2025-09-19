Feature: Copy and Paste

   Background:
 
   Scenario: Valid JSON can be pasted and copied back
      When she puts the file contents "test/dict.json" on the clipboard
      And she sends key "p" to "root"
      And she waits 1 seconds
      And she invalidates the clipboard
      And she sends key "c" to "root"
      And the clipboard contains the file contents "test/dict.json"
