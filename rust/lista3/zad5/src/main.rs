fn main() {
    println!("Hello, world!");
}

fn number(bus_stops:&[(i32,i32)]) -> i32 {
    bus_stops.iter().map(|(a, b)| a - b).sum()
}

#[test]
fn returns_expected1() {
  assert_eq!(number(&[(10,0),(3,5),(5,8)]), 5);
}
#[test]
fn returns_expected2() {
  assert_eq!(number(&[(3,0),(9,1),(4,10),(12,2),(6,1),(7,10)]), 17);
}
#[test]
fn returns_expected3() {
  assert_eq!(number(&[(3,0),(9,1),(4,8),(12,2),(6,1),(7,8)]), 21);
}
#[test]
fn returns_expected4() {
    assert_eq!(number(&[(10,0),(0,5),(0,5)]), 0);
}
#[test]
fn returns_expected5() {
    assert_eq!(number(&[(10,0),(5,5),(5,5),(5,5),(5,5),(5,5),(5,5),(5,5),(5,5),(5,5),(5,5),(5,5),(5,5),(5,5),(5,1)]), 14);
}