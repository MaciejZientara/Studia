fn string_to_number(s: &str) -> i32 {
    s.parse().unwrap()
}


fn main() {
}


#[cfg(test)]
mod tests {
    use super::string_to_number;
    // use rand::prelude::*;

    #[test]
    fn returns_expected_1() {
      assert_eq!(string_to_number("1234"), 1234);
    }
    #[test]
    fn returns_expected_2() {
      assert_eq!(string_to_number("605"), 605);
    }
    #[test]
    fn returns_expected_3() {
      assert_eq!(string_to_number("1405"), 1405);
    }
    #[test]
    fn returns_expected_4() {
      assert_eq!(string_to_number("-7"), -7);
    }
    #[test]
    fn returns_expected_5() {
      assert_eq!(string_to_number("-220"), -220);
    }
    

    // #[test]
    // fn works_on_random() {
    //     let mut rng = thread_rng();
    //     for _ in 0..5 {
    //         let num : i32 = rng.gen();
    //         let input = num.to_string();
    //         assert_eq!(string_to_number(&input), num);
    //     }        
    // }
}