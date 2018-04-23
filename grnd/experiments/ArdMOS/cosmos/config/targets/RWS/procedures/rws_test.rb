# To add this test to Test Runner edit config/tools/test_runner/test_runner.txt
# Add this line:
#   REQUIRE_UTILITY 'rws_test'
#
# Test Runner test script
class RwsTest < Cosmos::Test
  # def setup
  #   # Implement group level setup
  # end

  def test_command
    cmd("RWS COMMAND")
    wait_check("RWS STATUS BOOL == 'FALSE'", 5)
  end

  # def teardown
  #   # Implement group level teardown
  # end
end

class RwsSuite < Cosmos::TestSuite
  # def setup
  #   # Implement suite level setup
  # end

  def initialize
    super()
    add_test('RwsTest')
  end

  # def teardown
  #   # Implement suite level teardown
  # end
end
