# To add this test to Test Runner edit config/tools/test_runner/test_runner.txt
# Add this line:
#   REQUIRE_UTILITY 'matlab_test'
#
# Test Runner test script
class MatlabTest < Cosmos::Test
  # def setup
  #   # Implement group level setup
  # end

  def test_command
    cmd("MATLAB COMMAND")
    wait_check("MATLAB STATUS BOOL == 'FALSE'", 5)
  end

  # def teardown
  #   # Implement group level teardown
  # end
end

class MatlabSuite < Cosmos::TestSuite
  # def setup
  #   # Implement suite level setup
  # end

  def initialize
    super()
    add_test('MatlabTest')
  end

  # def teardown
  #   # Implement suite level teardown
  # end
end
