fn main() {
    println!("Hello, world!");
}

fn zoom(n: i32) -> String {
  let s1 = if (((n/2)+1 as i32)%2)==1 {"■"} else {"□"};
  let s2 = if (((n/2)+1 as i32)%2)==1 {"□"} else {"■"};
  println!("s1={} s2={}",s1,s2);
  let mut symbol;
  let mut res = "".to_owned();
  for i in 0..n {
    symbol = if (i%2)==1 {s2} else {s1};
    for j in 0..=n {
      if j==0 {
        res.push_str(s1);
        symbol = s2;
      }
      else if ((j>=i) && (j<(n-i))) || ((j<=i) && (j>(n-i-1))) {
        res.push_str(if (i%2)==0 {s1} else {s2});
        symbol = if (i%2)==1 {s1} else {s2};
      }
      else if j==n {
        if i!=(n-1) {
          res.push_str("\n");
        }
      }
      else {
        res.push_str(symbol);
        symbol = if symbol=="□" {"■"} else {"□"};
      }
    }
  }
  res
}


#[test]
fn basic_test_1() {
  assert_eq!(zoom(1), "■");
}

#[test]
fn basic_test_2() {
  assert_eq!(zoom(3), "\
□□□
□■□
□□□"
  );
}

#[test]
fn basic_test_3() {
  assert_eq!(zoom(5), "\
■■■■■
■□□□■
■□■□■
■□□□■
■■■■■"
  );
}

#[test]
fn basic_test_4() {
  assert_eq!(zoom(7), "\
□□□□□□□
□■■■■■□
□■□□□■□
□■□■□■□
□■□□□■□
□■■■■■□
□□□□□□□"
  );
}

#[test]
fn basic_test_5() {
  assert_eq!(zoom(9), "\
■■■■■■■■■
■□□□□□□□■
■□■■■■■□■
■□■□□□■□■
■□■□■□■□■
■□■□□□■□■
■□■■■■■□■
■□□□□□□□■
■■■■■■■■■"
  );
}