# To add this test to Test Runner edit config/tools/test_runner/test_runner.txt
# Add this line:
#   REQUIRE_UTILITY 'ps_dp832a_test'
#
# Test Runner test script
class Ps_dp832aTest < Cosmos::Test
  # def setup
  #   # Implement group level setup
  # end

  def test_command
    cmd("PS_DP832A COMMAND")
    wait_check("PS_DP832A STATUS BOOL == 'FALSE'", 5)
  end

  # def teardown
  #   # Implement group level teardown
  # end
end

class Ps_dp832aSuite < Cosmos::TestSuite
  # def setup
  #   # Implement suite level setup
  # end

  def initialize
    super()
    add_test('Ps_dp832aTest')
  end

  # def teardown
  #   # Implement suite level teardown
  # end
end
