use std::io;
use std::str::Chars;

struct Node {
    n: u32,
    l0: Option<Box<Node>>,
    l1: Option<Box<Node>>,
}

impl Node {
    fn new() -> Node {
        Node {
            n: 0,
            l0: None,
            l1: None,
        }
    }
    fn push<'a>(&mut self, iter: &mut Chars<'a>) -> () {
        match iter.next() {
            None => (),
            Some('0') => {
                if self.l0.is_none() {
                    self.l0 = Some(Box::new(Node::new()));
                }
                self.l0.as_mut().unwrap().n += 1;
                self.l0.as_mut().unwrap().push(iter);
            },
            Some('1') => {
                if self.l1.is_none() {
                    self.l1 = Some(Box::new(Node::new()));
                }
                self.l1.as_mut().unwrap().n += 1;
                self.l1.as_mut().unwrap().push(iter);
            },
            Some(_) => (),
        }
    }
}

fn main() -> io::Result<()> {
    let mut v0 = Vec::new();
    let mut v1 = Vec::new();
    let mut line = String::new();
    let mut gamma = 0;
    let mut epsilon = 0;
    let mut view = Box::new(Node::new());
    io::stdin().read_line(&mut line);
    for c in line.chars() {
        match c {
            '0' => {
                v0.push(1);
                v1.push(0);
            },
            '1' => {
                v0.push(0);
                v1.push(1);
            },
            _ => break,
        }
    }
    view.push(&mut line.chars());
    loop {
        let mut line = String::new();
        match io::stdin().read_line(&mut line) {
            Ok(0) => break,
            Err(_) => break,
            Ok(_) => {
                for (i, c) in line.chars().enumerate() {
                    match c {
                        '0' => {
                            v0[i] += 1;
                        },
                        '1' => {
                            v1[i] += 1;
                        },
                        _ => break,
                    }
                }
                view.push(&mut line.chars());
            }
        }
    }
    let mut oxy = 0;
    let mut carbonic = 0;
    let mut voxy = Some(&view);
    let mut vcarbon = Some(&view);
    for (i, v) in v0.iter().enumerate() {
        gamma *= 2;
        epsilon *= 2;
        if v > &v1[i] {
            epsilon += 1;
        } else {
            gamma += 1;
        }
    }

    while voxy.is_some() {
        if voxy.as_ref().unwrap().l0.is_none() &&
            voxy.as_ref().unwrap().l1.is_none() {
                break;
            }
        oxy *= 2;
        carbonic *= 2;
        if voxy.as_ref().unwrap().l0.is_none() {
            voxy = voxy.as_ref().unwrap().l1.as_ref();
            oxy += 1;
        } else if voxy.as_ref().unwrap().l1.is_none() {
            voxy = voxy.as_ref().unwrap().l0.as_ref();
        } else {
            let o0 = voxy.as_ref().unwrap().l0.as_ref().unwrap().n;
            let o1 = voxy.as_ref().unwrap().l1.as_ref().unwrap().n;
            if o0 > o1 {
                voxy = voxy.as_ref().unwrap().l0.as_ref();
            } else {
                voxy = voxy.as_ref().unwrap().l1.as_ref();
                oxy += 1;
            }
        }
        if vcarbon.as_ref().unwrap().l0.is_none() {
            vcarbon = vcarbon.as_ref().unwrap().l1.as_ref();
            carbonic += 1;
        } else if vcarbon.as_ref().unwrap().l1.is_none() {
            vcarbon = vcarbon.as_ref().unwrap().l0.as_ref();
        } else {
            let c0 = vcarbon.as_ref().unwrap().l0.as_ref().unwrap().n;
            let c1 = vcarbon.as_ref().unwrap().l1.as_ref().unwrap().n;
            if c0 > c1 {
                vcarbon = vcarbon.as_ref().unwrap().l1.as_ref();
                carbonic += 1;
            } else {
                vcarbon = vcarbon.as_ref().unwrap().l0.as_ref();
            }
        }
    }
    println!("{}", gamma * epsilon);
    println!("{}", oxy * carbonic);
    Ok(())
}
