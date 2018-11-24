Import("env")

# please keep $SOURCE variable, it will be replaced with a path to firmware

# In-line command with arguments
env.Replace(
    UPLOADCMD="avrdude -p m328p -c avrisp2 -U flash:w:$SOURCE"
)

# # Python callback
# def on_upload(source, target, env):
#     print source, target
#     firmware_path = str(source[0])
#     # do something
#     env.Execute("executable arg1 arg2")

# env.Replace(UPLOADCMD=on_upload)