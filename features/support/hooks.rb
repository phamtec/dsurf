require "service_manager"

After do |scenario|
   ServiceManager.stop
   ServiceManager.start
end

