# To add this test to Test Runner edit config/tools/test_runner/test_runner.txt
# Add this line:
#   REQUIRE_UTILITY 'visa_dm3058e_usb_test'
#
# Test Runner test script
class Visa_dm3058e_usbTest < Cosmos::Test
  # def setup
  #   # Implement group level setup
  # end

  def test_command
    cmd("VISA_DM3058E_USB COMMAND")
    wait_check("VISA_DM3058E_USB STATUS BOOL == 'FALSE'", 5)
  end

  # def teardown
  #   # Implement group level teardown
  # end
end

class Visa_dm3058e_usbSuite < Cosmos::TestSuite
  # def setup
  #   # Implement suite level setup
  # end

  def initialize
    super()
    add_test('Visa_dm3058e_usbTest')
  end

  # def teardown
  #   # Implement suite level teardown
  # end
end
