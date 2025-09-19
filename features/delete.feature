Feature: Elements can be deleted

   Background:
 
   Scenario: Lists can be deleted
      When she puts the file contents "test/listoflists.json" on the clipboard
      And she sends key "p" to "/"
      And she waits 0.5 seconds
      And "/" contains 3 elements
      And she sends key "d" to "/1"
      And she waits 0.5 seconds
      Then "/" contains 2 elements
 