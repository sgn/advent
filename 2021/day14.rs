use std::io;
use std::collections::HashMap;
use std::hash::Hash;
use std::mem;

/*
fn expand(map: &HashMap::<u16, char>, template: &mut LinkedList::<char>) {
    let mut cursor = template.cursor_front_mut();
    let mut prev = *cursor.current().unwrap();
    loop {
        cursor.move_next();
        match cursor.current() {
            None => break,
            Some(&mut c) => {
                let key = prev as u16 * 256 + c as u16;
                match map.get(&key) {
                    None => (),
                    Some(value) => cursor.insert_before(*value),
                }
                prev = c;
            },
        }
    }
}

fn minmax(template: &LinkedList::<char>) {
    let mut ccount = HashMap::<char, u64>::new();
    for c in template.iter() {
        match ccount.get(&c) {
            None => { ccount.insert(*c, 1); },
            Some(&x) => { ccount.insert(*c, x + 1); },
        }
    }
    let min = ccount.values().min().unwrap();
    let max = ccount.values().max().unwrap();
    println!("{}", max - min);
}

fn main() -> io::Result<()> {
    let mut line = String::new();
    io::stdin().read_line(&mut line);
    let mut template = LinkedList::new();
    for c in line.chars() {
        match c {
            '\n' => (),
            x => template.push_back(x),
        }
    }
    let mut map = HashMap::<u16, char>::new();
    loop {
        let mut line = String::new();
        match io::stdin().read_line(&mut line) {
            Ok(0) | Err(_) => break,
            Ok(1) => (),
            Ok(_) => {
                let mut it = line.trim_end().split(" -> ");
                let pair = it.next().unwrap();
                let mut xit = pair.chars();
                let key = xit.next().unwrap() as u16 * 256 +
                    xit.next().unwrap() as u16;
                map.insert(key, it.next().unwrap().chars().next().unwrap());
            }
        }
    }
    for _ in 0..10 {
        expand(&map, &mut template);
    }
    minmax(&template);
    for _ in 10..40 {
        expand(&map, &mut template);
    }
    minmax(&template);
    Ok(())
}
*/

fn do_append<K, V>(map: &mut HashMap::<K, V>, key: K, value: V)
    where K: Eq, K: Hash, V: Copy, V: std::ops::Add<Output = V>
{
    match map.get(&key) {
        None => { map.insert(key, value); },
        Some(&old) => { map.insert(key, value + old); },
    }
}

fn make_key(lhs: u16, rhs: u16) -> u16 {
    (lhs << 8) | rhs
}

fn expand(template: &mut HashMap::<u16, usize>,
          map: &HashMap::<u16, char>) {
    let mut tmp = HashMap::<u16, usize>::new();
    for (&key, &value) in template.iter() {
        match map.get(&key) {
            None => do_append(&mut tmp, key, value),
            Some(&c) => {
                let lhs = make_key(key >> 8, c as u16);
                let rhs = make_key(c as u16, key & 0xFF);
                do_append(&mut tmp, lhs, value);
                do_append(&mut tmp, rhs, value);
            }
        }
    }
    mem::swap(template, &mut tmp);
}

fn output(template: &HashMap::<u16, usize>, last: u16) {
    let mut m = HashMap::new();
    do_append(&mut m, last, 1);
    for (&key, &value) in template.iter() {
        do_append(&mut m, key >> 8, value);
    }
    let min = m.values().min().unwrap();
    let max = m.values().max().unwrap();
    println!("{}", max - min);
}

fn main() -> io::Result<()> {
    let mut line = String::new();
    io::stdin().read_line(&mut line);
    let mut template = HashMap::<u16, usize>::new();
    let mut chars = line.chars();
    let mut last = chars.next().unwrap() as u16;
    for c in chars {
        match c {
            '\n' => (),
            _ => {
                let rhs = c as u16;
                let key = make_key(last, rhs);
                do_append(&mut template, key, 1);
                last = rhs;
            }
        }
    }
    let mut map = HashMap::<u16, char>::new();
    loop {
        let mut line = String::new();
        match io::stdin().read_line(&mut line) {
            Ok(0) | Err(_) => break,
            Ok(1) => (),
            Ok(_) => {
                let mut it = line.trim_end().split(" -> ");
                let pair = it.next().unwrap();
                let mut xit = pair.chars();
                let key = make_key(
                    xit.next().unwrap() as u16,
                    xit.next().unwrap() as u16);
                map.insert(key, it.next().unwrap().chars().next().unwrap());
            }
        }
    }
    for _ in 0..10 {
        expand(&mut template, &map);
    }
    output(&template, last);
    for _ in 10..40 {
        expand(&mut template, &map);
    }
    output(&template, last);
    Ok(())
}
