Feature: Elements can be deleted

   Background:
 
   Scenario: Lists can be deleted
      When she puts the file contents "test/listoflists.json" on the clipboard
      And she sends key "p" to "/"
      And "/" contains 3 elements
      And she sends key "d" to "/1"
      Then "/" contains 2 elements
 
    Scenario: Multiple deletes can happen in a row
      When she puts the file contents "test/listoflists.json" on the clipboard
      And she sends key "p" to "/"
      And "/" contains 3 elements
      And she sends key "d" to "/1"
      And "/" contains 2 elements
      And she sends key "d" to "/1"
      Then "/" contains 1 elements
      And she sends key "d" to "/0"
      Then "/" contains 0 elements
 
    Scenario: Dicts can be deleted
      When she puts the file contents "test/list.json" on the clipboard
      And she sends key "p" to "/"
      And "/" contains 3 elements
      And she sends key "d" to "/1"
      Then "/" contains 2 elements
 
