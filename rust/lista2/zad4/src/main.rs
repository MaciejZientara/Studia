fn main() {
    println!("Hello, world!");
}

fn longest(a1: &str, a2: &str) -> String {
    let mut a = a1.chars().collect::<Vec<char>>();
    let mut b = a2.chars().collect::<Vec<char>>();
    a.append(&mut b);
    a.sort();
    a.dedup();
    String::from_iter(a)
}



#[cfg(test)]
    mod tests {
    use super::*;
   
    fn testing(s1: &str, s2: &str, exp: &str) -> () {
        println!("s1:{:?} s2:{:?}", s1, s2);
        println!("{:?} {:?}", longest(s1, s2), exp);
        println!("{}", longest(s1, s2) == exp);
        assert_eq!(&longest(s1, s2), exp)
    }

    #[test]
    fn basic_tests1() {
        testing("aretheyhere", "yestheyarehere", "aehrsty");
    }
    #[test]
    fn basic_tests2() {
        testing("loopingisfunbutdangerous", "lessdangerousthancoding", "abcdefghilnoprstu");
    }
    #[test]
    fn basic_tests3() {
        testing("ayyyyyyyyy", "byyyyyyyy", "aby");
    }
    #[test]
    fn basic_tests4() {
        testing("bac", "cba", "abc");
    }
    #[test]
    fn basic_tests5() {
        testing("asdfg", "zxcvb", "abcdfgsvxz");
    }
}
