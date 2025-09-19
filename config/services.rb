require 'service_manager'

ServiceManager.define_service 'dsurf' do |s|
   s.start_cmd = 'cd build; ./dsurf'
   s.loaded_cue = /Local REP/
   s.cwd = Dir.pwd
   s.pid_file = 'dsurf.pid'
end
