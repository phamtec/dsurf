Feature: New elements can be created

   Background:
 
   Scenario: A new string can be created
      When she sends key "n" to "root"
      And she sends key "s" to "root"
      And she waits 1 seconds
      Then "root" contains 1 elements
  
   Scenario: A new bool can be created
      When she sends key "n" to "root"
      And she sends key "b" to "root"
      And she waits 1 seconds
      Then "root" contains 1 elements
  
   Scenario: A new number can be created
      When she sends key "n" to "root"
      And she sends key "n" to "root"
      And she waits 1 seconds
      Then "root" contains 1 elements
  
   Scenario: A new dict can be created
      When she sends key "n" to "root"
      And she sends key "d" to "root"
      And she waits 1 seconds
      Then "root" contains 1 elements

   Scenario: A new list can be created
      When she sends key "n" to "root"
      And she sends key "l" to "root"
      And she waits 1 seconds
      Then "root" contains 1 elements
  