use std::io;
use std::collections::HashMap;
use std::collections::HashSet;

fn do_insert(map: &mut HashMap::<String, HashSet::<String>>,
             lhs: &str, rhs: &str) {
    if !map.contains_key(&lhs.to_string()) {
        map.insert(lhs.to_string(), HashSet::<String>::new());
    }
    map.get_mut(&lhs.to_string()).unwrap().insert(rhs.to_string());
}

fn do_count_path(map: &HashMap::<String, HashSet::<String>>,
                 small: &mut Vec::<String>, vertice: &String) -> usize {
    let mut pushed = false;
    if vertice == "end" {
        return 1;
    }
    if vertice.chars().next().unwrap().is_lowercase() {
        if small.contains(vertice) {
            return 0;
        }
        pushed = true;
        small.push(vertice.clone());
    }
    let mut num = 0;
    for v in map[vertice].iter() {
        num += do_count_path(&map, small, v);
    }
    if pushed { small.pop(); }
    return num;
}

fn count_path(map: &HashMap::<String, HashSet::<String>>) -> usize {
    let mut small = Vec::<String>::new();
    do_count_path(&map, &mut small, &"start".to_string())
}

fn do_count_neo(map: &HashMap::<String, HashSet::<String>>,
                small: &mut Vec::<String>, vertice: &String) -> usize {
    let mut pushed = false;
    if vertice == "end" {
        return 1;
    }
    if vertice.chars().next().unwrap().is_lowercase() {
        if vertice != "start" && small.contains(vertice) {
            let mut num = 0;
            for v in map[vertice].iter() {
                num += do_count_path(&map, small, v);
            }
            return num;
        }
        pushed = true;
        small.push(vertice.clone());
    }
    let mut num = 0;
    for v in map[vertice].iter() {
        num += do_count_neo(&map, small, v);
    }
    if pushed { small.pop(); }
    return num;
}

fn count_path_neo(map: &HashMap::<String, HashSet::<String>>) -> usize {
    let mut small = Vec::<String>::new();
    do_count_neo(&map, &mut small, &"start".to_string())
}

fn main() -> io::Result<()> {
    let mut map = HashMap::new();
    loop {
        let mut line = String::new();
        match io::stdin().read_line(&mut line) {
            Ok(0) | Err(_) => break,
            Ok(_) => {
                let mut it = line.trim_end().split("-");
                let lhs = it.next().unwrap();
                let rhs = it.next().unwrap();
                if lhs != "end" && rhs != "start" {
                    do_insert(&mut map, &lhs, &rhs);
                }
                if rhs != "end" && lhs != "start" {
                    do_insert(&mut map, &rhs, &lhs);
                }
            }
        }
    }
    let n = count_path(&map);
    println!("{}", n);
    let n = count_path_neo(&map);
    println!("{}", n);
    Ok(())
}
