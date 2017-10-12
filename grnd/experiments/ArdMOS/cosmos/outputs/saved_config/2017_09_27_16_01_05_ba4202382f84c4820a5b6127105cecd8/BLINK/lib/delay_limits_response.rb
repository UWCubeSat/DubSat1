require 'cosmos/packets/limits_response'

class DelayLimitsResponse < Cosmos::LimitsResponse

	def call(packet, item, old_limits_state)
		case item.limits.state
		when :GREEN
			cmd('BLINK', 'DELAY', 'DELAY' => 128)
		when :RED_HIGH
			cmd('BLINK', 'DELAY', 'DELAY' => 149)
		when :RED_LOW
			cmd('BLINK', 'DELAY', 'DELAY' => 51)
		end
	end

end
