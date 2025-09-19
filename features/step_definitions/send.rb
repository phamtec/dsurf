require 'json'

# Send JSON to Nodes, regturninbg resulting JSON.

def Send(j)
   repPort = 3013
   s = JSON.generate(j)
   return JSON.parse(`build/Send '#{s}'`)
end
