fn main() {
    println!("Hello, world!");
}

fn is_vowel(c : char) -> bool {
    c=='a' || c=='i' || c=='u' || c=='e' || c=='o'
}

fn get_count(string: &str) -> usize {
    // let mut vowels_count: usize = 0;
    string.chars().filter(|c| is_vowel(*c)).count()
    // vowels_count
}
  
#[test]
fn my_tests1() {
  assert_eq!(get_count("abracadabra"), 5);
}
#[test]
fn my_tests2() {
  assert_eq!(get_count("aiueo"), 5);
}
#[test]
fn my_tests3() {
  assert_eq!(get_count("brtwgfj"), 0);
}
#[test]
fn my_tests4() {
  assert_eq!(get_count("alfa"), 2);
}
#[test]
fn my_tests5() {
  assert_eq!(get_count("beta"), 2);
}