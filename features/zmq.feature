
@broken
Feature: ZMQ

   Background:
 
   Scenario: A project with ZMQ can be used
      When she puts the file contents "modules/project/test/project.json" on the clipboard
      And she sends key "p" to "/"
      And she sends key "c" to "Streams"
      And "<zmq-result>/" contains 3 elements
