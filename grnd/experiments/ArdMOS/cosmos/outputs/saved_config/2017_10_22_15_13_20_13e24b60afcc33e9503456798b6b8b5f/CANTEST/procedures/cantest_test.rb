# To add this test to Test Runner edit config/tools/test_runner/test_runner.txt
# Add this line:
#   REQUIRE_UTILITY 'cantest_test'
#
# Test Runner test script
class CantestTest < Cosmos::Test
  # def setup
  #   # Implement group level setup
  # end

  def test_command
    cmd("CANTEST COMMAND")
    wait_check("CANTEST STATUS BOOL == 'FALSE'", 5)
  end

  # def teardown
  #   # Implement group level teardown
  # end
end

class CantestSuite < Cosmos::TestSuite
  # def setup
  #   # Implement suite level setup
  # end

  def initialize
    super()
    add_test('CantestTest')
  end

  # def teardown
  #   # Implement suite level teardown
  # end
end
