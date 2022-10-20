fn main() {
    println!("Hello, world!");
}

fn solution(num: i32) -> i32 {
    (1..num).filter(|i| i%3==0 || i%5==0).sum()
}

mod tests {
    use super::solution;
    
    #[test]
    fn sample_test1() {
      // assertion(expected, input);
      assertion(23, 10);
    }
    #[test]
    fn sample_test2() {
      // assertion(expected, input);
      assertion(33, 11);
    }
    #[test]
    fn sample_test3() {
      // assertion(expected, input);
      assertion(225, 33);
    }
    #[test]
    fn sample_test4() {
      // assertion(expected, input);
      assertion(8, 6);
    }
    #[test]
    fn sample_test5() {
      // assertion(expected, input);
      assertion(3420, 123);
    }
    #[test]
    fn sample_test6() {
      // assertion(expected, input);
      assertion(543, 50);
    }
    #[test]
    fn sample_test7() {
      // assertion(expected, input);
      assertion(25719750, 10500);
    }
    
    fn assertion(expected : i32, input : i32) {
        let actual = solution(input);
        
        assert!(
            expected == actual,
            "\nTest failed!\n expected: {}\n actual: {}\n input: {}\n", expected, actual, input
        );
    }
}