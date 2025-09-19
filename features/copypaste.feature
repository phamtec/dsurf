Feature: Copy and Paste

   Background:
 
   Scenario: Valid JSON can be pasted and copied back
      When she puts the file contents "test/dict.json" on the clipboard
      And she sends key "p" to "/"
      And she waits 0.5 seconds
      And she invalidates the clipboard
      And she sends key "c" to "/"
      And the clipboard contains the file contents "test/dict.json"
