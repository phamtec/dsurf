Feature: Changes can be undone to lists

   Undo and redo always go to root.
   
   Background:
      When she puts the file contents "test/listoflists.json" on the clipboard
      And she sends key "p" to "/"
      And "/" contains 3 elements
 
   Scenario: Undo cant be done when nothing to undo
      And she sends key "u" to "/"
      Then "/" contains 3 elements

   Scenario: Redo cant be done when nothing to undo
      And she sends key "r" to "/"
      Then "/" contains 3 elements

   Scenario: Lists deletes can be undone
      And she sends key "d" to "/1"
      Then "/" contains 2 elements
      And she sends key "u" to "/"
      Then "/" contains 3 elements
       And she sends key "r" to "/"
      Then "/" contains 2 elements

   Scenario: Multiple List deletes can be undone
      And she sends key "d" to "/1"
      Then "/" contains 2 elements
      And she sends key "d" to "/1"
      Then "/" contains 1 elements
      And she sends key "u" to "/"
      Then "/" contains 2 elements
      And she sends key "u" to "/"
      Then "/" contains 3 elements

   Scenario: A new string can be undone
      When she sends key "n" to "/"
      And she sends key "s" to "/"
      Then "/" contains 4 elements
      And she sends key "u" to "/"
      Then "/" contains 3 elements
  
