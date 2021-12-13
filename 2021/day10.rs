use std::io;

fn main() -> io::Result<()> {
    let mut escore : u64 = 0;
    let mut cscore = Vec::<u64>::new();
    loop {
        let mut line = String::new();
        match io::stdin().read_line(&mut line) {
            Err(_) | Ok(0) => break,
            Ok(_) => {
                let mut stack = Vec::<char>::new();
                stack.reserve(line.len() / 2);
                for c in line.chars() {
                    match c {
                        '(' | '[' | '{' | '<' => stack.push(c),
                        ')' =>
                            if stack.pop().unwrap() != '(' {
                                escore += 3;
                                break;
                            },
                        ']' =>
                            if stack.pop().unwrap() != '[' {
                                escore += 57;
                                break;
                            },
                        '}' =>
                            if stack.pop().unwrap() != '{' {
                                escore += 1197;
                                break;
                            },
                        '>' =>
                            if stack.pop().unwrap() != '<' {
                                escore += 25137;
                                break;
                            },
                        '\n' =>
                            cscore.push(
                                stack.iter()
                                .rfold(0, |a, e|
                                       a as u64 * 5 as u64 +
                                       if *e == '(' { 1 }
                                       else if *e == '[' { 2 }
                                       else if *e == '{' { 3 }
                                       else { 4 })),
                        _ => panic!("xxxx"),
                    }
                }
            }
        }
    }
    println!("{}", escore);
    cscore.sort();
    println!("{}", cscore[cscore.len() / 2]);
    Ok(())
}
