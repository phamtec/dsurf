require 'json'

$lastResult = nil

Then('she says {string}') do |msg|
  puts msg
end

When('she sends {string}') do |msg|
   $lastResult = Send({ "type": "msg", "msg": msg })
end

Then('she receives ack') do
   expect($lastResult["type"]).to eq("ack")
end
