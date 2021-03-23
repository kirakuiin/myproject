print(math.abs(3))
a = {x = 1, y = 2}
print(a.x, a.y)
a = {1, 2, 3, 4}
table.insert(a, 5)
table.insert(a, 1, 6)
for k, v in pairs(a) do
  print(k, v)
end
function is_valid_table(t)
  index = 1
  for k, _ in ipairs(t) do
    print(k)
    if k ~= index then
      return false
    else
      index = 1 + index
    end
  end
  return true
end

print(is_valid_table({[1]=1, [3]=3}))
