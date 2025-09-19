#!/bin/bash
# 
# Run the cucumber tests

bundle exec cucumber $1  --tags "not (@broken)" -r features/step_definitions -r features/support
