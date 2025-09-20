Feature: Changes can be undone

   Background:
 
   Scenario: Lists deletes can be undone
      When she puts the file contents "test/listoflists.json" on the clipboard
      And she sends key "p" to "/"
      And "/" contains 3 elements
      And she sends key "d" to "/1"
      Then "/" contains 2 elements
      And she sends key "u" to "/1"
      Then "/" contains 3 elements
       And she sends key "r" to "/1"
      Then "/" contains 2 elements
