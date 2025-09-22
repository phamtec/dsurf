Feature: Changes can be undone to dictionaries

   Undo and redo always go to root.
   
   Background:
      When she puts the file contents "test/dict.json" on the clipboard
      And she sends key "p" to "/"
      And "/" contains 3 elements
 
   Scenario: Dict deletes can be undone
      And she sends key "d" to "/1"
      Then "/" contains 2 elements
      And she sends key "u" to "/"
      Then "/" contains 3 elements
       And she sends key "r" to "/"
      Then "/" contains 2 elements

   Scenario: Multiple Dict deletes can be undone
      And she sends key "d" to "/1"
      Then "/" contains 2 elements
      And she sends key "d" to "/1"
      Then "/" contains 1 elements
      And she sends key "u" to "/"
      Then "/" contains 2 elements
      And she sends key "u" to "/"
      Then "/" contains 3 elements
  
   Scenario: A new string in a dict can be undone
      When she sends key "n" to "/"
      And she sends key "s" to "/"
      Then "/" contains 4 elements
      And she sends key "u" to "/"
      Then "/" contains 3 elements
  
