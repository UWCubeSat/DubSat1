# To add this test to Test Runner edit config/tools/test_runner/test_runner.txt
# Add this line:
#   REQUIRE_UTILITY 'can_local_test'
#
# Test Runner test script
class Can_localTest < Cosmos::Test
  # def setup
  #   # Implement group level setup
  # end

  def test_command
    cmd("CAN_LOCAL COMMAND")
    wait_check("CAN_LOCAL STATUS BOOL == 'FALSE'", 5)
  end

  # def teardown
  #   # Implement group level teardown
  # end
end

class Can_localSuite < Cosmos::TestSuite
  # def setup
  #   # Implement suite level setup
  # end

  def initialize
    super()
    add_test('Can_localTest')
  end

  # def teardown
  #   # Implement suite level teardown
  # end
end
