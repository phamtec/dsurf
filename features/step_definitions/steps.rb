require 'json'

$cadence = 0.1

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

def isMac()
   return RUBY_PLATFORM =~ /darwin/i
end

When('she puts the file contents {string} on the clipboard') do |filename|
   if isMac()
      `cat #{filename} | pbcopy`
   else
      `cat #{filename} | xclip -sel clip`
   end
end

When('she invalidates the clipboard') do
   if isMac()
      `echo "xx" | pbcopy`
   else
      `echo "xx" | xclip -sel clip`
   end
end

Then('the clipboard contains the file contents {string}') do |filename|
   json = `cat #{filename}`
   if isMac()
      clip_text = `pbpaste`
   else
      clip_text = `xsel`
   end
   expect(json).to eq(clip_text)
end

