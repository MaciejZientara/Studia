fn main() {
    println!("Hello, world!");
    camel_case("test case");
    camel_case("camel case method");
    camel_case("say hello ");
}

fn str_cap(s: &str) -> String {
    format!("{}{}", s.chars().next().unwrap().to_uppercase(), 
    s.chars().skip(1).collect::<String>())
}

fn camel_case(str: &str) -> String {
    let split = str.split(' ');
    let mut res = "".to_owned();
    for s in split {
        if !s.is_empty(){
            res.push_str(&str_cap(s));
        }
    }
    println!("{}", res);
    return res.to_string();
}

// Rust tests
#[test]
fn sample_test1() {
  assert_eq!(camel_case("test case"), "TestCase");
}
#[test]
fn sample_test2() {
  assert_eq!(camel_case("camel case method"), "CamelCaseMethod");
}
#[test]
fn sample_test3() {
  assert_eq!(camel_case("say hello "), "SayHello");
}
#[test]
fn sample_test4() {
  assert_eq!(camel_case(" camel case word"), "CamelCaseWord");
}
#[test]
fn sample_test5() {
  assert_eq!(camel_case(""), "");
}