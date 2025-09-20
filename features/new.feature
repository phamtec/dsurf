Feature: New elements can be created

   Background:
 
   Scenario: A new string can be created
      When she sends key "n" to "/"
      And she sends key "s" to "/"
      Then "/" contains 1 elements
  
   Scenario: A new bool can be created
      When she sends key "n" to "/"
      And she sends key "b" to "/"
      Then "/" contains 1 elements
  
   Scenario: A new number can be created
      When she sends key "n" to "/"
      And she sends key "n" to "/"
      Then "/" contains 1 elements
  
   Scenario: A new dict can be created
      When she sends key "n" to "/"
      And she sends key "d" to "/"
      Then "/" contains 1 elements

   Scenario: A new list can be created
      When she sends key "n" to "/"
      And she sends key "l" to "/"
      Then "/" contains 1 elements
  