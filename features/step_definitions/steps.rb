require 'json'

$cadence = 0.5

When('she sends key {string} to {string}') do |key, target|
   result = Send({ "type": "key", "target": target, "payload": key })
   expect(result["type"]).to eq("ack")
   sleep($cadence)
end

Then('{string} contains {int} elements') do |target, count|
   result = Send({ "type": "count", "target": target })
   expect(result["type"]).to eq("count")
   expect(result["payload"].to_i).to eq(count)
end

Then('she waits {int} seconds') do |n|
  sleep(n)
end

When('she puts the file contents {string} on the clipboard') do |filename|
   `cat #{filename} | pbcopy`
end

When('she invalidates the clipboard') do
   `echo "xx" | pbcopy`
end

Then('the clipboard contains the file contents {string}') do |filename|
   json = `cat #{filename}`
   clip_text = `pbpaste`
   expect(json).to eq(clip_text)
end

